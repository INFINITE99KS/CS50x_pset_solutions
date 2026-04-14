document.addEventListener('DOMContentLoaded', function()
{
    let input = document.querySelector('#editor');
    let text = document.querySelector('to_be_edited');
    document.addEventListener('keyup', function(){
        let text = document.querySelector('#to_be_edited');
        if(input.value)
        {
            text.innerHTML = `Welcome to my humble abode, ${input.value}!`;
            // document.querySelector('#hiddenParagraph').hidden = false;
            document.querySelector('#hiddenParagraph').style.opacity = 1;
            document.querySelector('#to_be_edited').style.transform = "translateY(0px)";
        }
        else
        {
            text.innerHTML = `Welcome to my humble abode!`;
            // document.querySelector('#hiddenParagraph').hidden = true;
            document.querySelector('#hiddenParagraph').style.opacity = 0;
            document.querySelector('#to_be_edited').style.transform = "translateY(20px)";
        }});
});

let board = new Array(9).fill(null);
let currentPlayer = `X`;
let gameOver = false;
let winner = null;
let winningConditions = [[0,1,2], [3,4,5] , [6,7,8], [0,4,8], [2,4,6], [0,3,6], [1,4,7], [2,5,8]];
let playerX = [];
let playerY = [];

function occupy(event)
{
    let trigger = event.srcElement;
    let trigger_index = trigger.dataset.index;
    if(gameOver)
    {
        alert(`GAME IS OVER!!!`);
        return;
    }
    if(board[trigger_index] === null)
    {
        board[trigger_index] = currentPlayer;
        if(currentPlayer === `X`) playerX.push(Number(trigger_index));
        else playerY.push(Number(trigger_index));
        trigger.innerHTML = currentPlayer;
        currentPlayer = (currentPlayer == `X`) ? `Y`:`X`;
        document.getElementById('playerturn').innerHTML = `It's ${currentPlayer}'s turn`;
        let winnerIntialCheck = checkWinner(playerX, `X`);
        winner = (winnerIntialCheck != null) ? winnerIntialCheck :checkWinner(playerY, `Y`);
        if(winner != null) gameOver = true;
        else gameOver = board.every(cell => cell !== null);
        checkState();
    }
    else
    {
        alert("Cell is occupied");
        return;
    }
}

function checkState()
{
    if(gameOver && winner == null)
            {
                alert("Game ended in a draw!");
                return;
            }
        else if (winner != null)
            {
                document.getElementById('playerturn').innerHTML = `${winner} Won!`;
                document.querySelector('.boardgame').style.filter = "brightness(0.8)";
            }
}

function checkWinner(playerMoves, charPlayer)
{
    for(condition of winningConditions)
    {
        let conditionFound = true;
        for(cell of condition)
        {
            if(!playerMoves.includes(cell))
            {
                conditionFound = false;
                break;
            }
        }
        if(conditionFound)
        {
            return charPlayer;
        }
    }
    return null;
}
