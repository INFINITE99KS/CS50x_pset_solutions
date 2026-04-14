import os
import datetime
import time
import random

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    shares = db.execute("SELECT * FROM user_shares WHERE userid = ? AND shares > 0", session["user_id"])
    shares_lookup = []
    for share in shares:
        symbol = share["symbol"]
        symbol_data = lookup(symbol)
        actual_share = {
            "symbol": share["symbol"],
            "shares": share["shares"],
            "price": symbol_data["price"],
            "total": symbol_data["price"]*share["shares"]
        }
        shares_lookup.append(actual_share)
    currentUser = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
    totalSharesCash = 0
    for share in shares_lookup:
        totalSharesCash += share["total"]
    totalCash = currentUser[0]["cash"] + totalSharesCash
    return render_template("index.html", shares = shares_lookup, userCash = currentUser[0]["cash"], totalCash = totalCash)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():

    if request.method == "POST":
       symbol = request.form.get("symbol")
       shares = request.form.get("shares")
       processed_symbol = lookup(symbol)

       if not processed_symbol:
           return apology("Invalid Symbol", 400)

    #    if shares == "":
    #        return apology("Please enter the number of shares.", 400)
    #    else:
    #     shares = int(shares)

    #    if shares <= 0:
    #         return apology("Please enter a greater than zero number.")
       if not shares or not shares.isdigit() or int(shares) <= 0:
            return apology("Please enter a +ve integer number.")
       shares = int(shares)

       current_user = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
       if processed_symbol["price"] * shares > current_user[0]["cash"]:
            return apology("Not enough cash.")
       else:
            # Succesful operation.
            db.execute("INSERT INTO transactions (userid, symbol, shares, date, price, transaction_id, type) VALUES (?, ?, ?, ?, ?, ?, ?)", session["user_id"], processed_symbol["symbol"],
                       shares, datetime.datetime.now(), processed_symbol["price"] * shares, generate_transaction_id(), "buy")

            # Deduct cash.
            user = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
            db.execute("UPDATE users SET cash = ? WHERE id = ?", float(user[0]["cash"]) - processed_symbol["price"] * shares, session["user_id"])

            check_symbols = db.execute("SELECT * FROM user_shares WHERE userid = ? AND symbol = ?", session["user_id"], symbol)

            # Add to sum of shares.
            if not check_symbols:
                db.execute("INSERT INTO user_shares (userid, symbol, shares) VALUES (?, ?, ?)", session["user_id"], symbol, shares)
            else:
                db.execute("UPDATE user_shares SET shares = shares + ? WHERE userid = ? AND symbol = ?", shares, session["user_id"], symbol)

            # Redirect user to home page
            return redirect("/")
    else:
        return render_template("buy.html")

def generate_transaction_id():
    timestamp = int(time.time()*1000)
    rand = random.randint(100, 999)
    return int(f"{timestamp}{rand}")

@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
    user_transactions = db.execute("SELECT * FROM transactions WHERE userid = ?", user[0]["id"])
    return render_template("history.html", transactions = user_transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        process_symbol = lookup(symbol)
        if not process_symbol:
            return apology("Invalid Symbol", 400)
        else:
            return render_template("quoted.html", name = process_symbol["name"], symbol = process_symbol["symbol"], price = process_symbol["price"])
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        username = request.form.get("username")
        if username == "":
            return apology("Username left empty.")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")
        if password != confirmation:
            return apology("Passwords Mismatch.")
        elif password == "":
            return apology("Password left empty.")
        else:
            password_hash = generate_password_hash(password, method='scrypt', salt_length=16)
        try:
            db.execute("INSERT INTO users (username, hash) VALUES (?,?)", username, password_hash)
        except ValueError:
            return apology("Username already exists")

        # Redirect user to home page
        return redirect("/")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    user_shares = db.execute("SELECT symbol, SUM(shares) FROM transactions WHERE userid = ? AND type = ? GROUP BY symbol", session["user_id"], "buy")
    user = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
    target_share = []
    symbol_found = False
    if request.method == "POST":
        symbol = request.form.get("symbol")
        for share in user_shares:
            if share["symbol"] == symbol:
                target_share = share
                symbol_found = True
                break
        if not symbol_found:
            return apology("Missing symbol.")
        shares = request.form.get("shares")
        if shares == "":
            return apology("Shares left empty.")
        else:
            shares = int(request.form.get("shares"))
        processed_symbol = lookup(symbol)
        if shares > target_share["SUM(shares)"]:
            return apology("Insufficient shares.")
        else:
            # Succesful operation.
            db.execute("INSERT INTO transactions (userid, symbol, shares, date, price, transaction_id, type) VALUES (?, ?, ?, ?, ?, ?, ?)", session["user_id"], processed_symbol["symbol"],
                       -shares, datetime.datetime.now(), (processed_symbol["price"] * shares), generate_transaction_id(), "sell")
            # Add cash.
            user = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
            db.execute("UPDATE users SET cash = ? WHERE id = ?", float(user[0]["cash"]) + processed_symbol["price"] * shares, session["user_id"])

            # Deduct sum of shares.
            db.execute("UPDATE user_shares SET shares = shares - ? WHERE userid = ? AND symbol = ?", shares, session["user_id"], symbol)

            # Redirect user to home page
            return redirect("/")
    else:
        return render_template("sell.html", user_shares = user_shares)

@app.route("/forgot", methods=["GET", "POST"])
def forgot():
    if request.method == "POST":
        username = request.form.get("username")
        try:
            check_user = db.execute("SELECT * FROM users WHERE username = ?", username)
            if not check_user:
                return apology("No such user exists.")
            password = request.form.get("password")
            confirmation = request.form.get("confirmation")
            if password == "":
                return apology("Please enter your password.")
            elif password != confirmation:
                return apology("Password mismatch.")
            else:
                pass_hash = generate_password_hash(password, method='scrypt', salt_length=16)
            db.execute("UPDATE users SET hash = ? WHERE username = ?", pass_hash, username)
            # Redirect user to home page
            return redirect("/")
        except ValueError:
            return apology("User doesn't exist")
    else:
        return render_template("forgot.html")
