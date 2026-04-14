-- Keep a log of any SQL queries you execute as you solve the mystery.
-- Details: | 295 | 2024 | 7     | 28  | Humphrey Street      | Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery.
-- Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery. |
-- The time was 10:15AM.
SELECT * FROM crime_scene_reports;
-- Then I got the interviews' transciprts via this query:
SELECT transcript FROM interviews WHERE year = 2024 AND month = 7 AND day = 28;
-- Ruth said, "Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away.
-- If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame. "

-- Eugene said, "I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery,
-- I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money."

-- Raymond said, "As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow.
-- The thief then asked the person on the other end of the phone to purchase the flight ticket. "

-- So now am gonna check the Parking Lot, the Phone calls (Also the accomplice), the ATM, the Bank (for the accomplice) as well as the Airport, and would put Eugene on the list of potential accomplices.
-- First, checking atm transactions that day,
SELECT FROM atm_transactions WHERE year = 2024 AND month = 7 AND day = 28 AND atm_location LIKE 'Humphrey%' AND transaction_type = 'withdraw';

-- Now onto bakery logs,
SELECT * FROM bakery_security_logs WHERE year = 2024 AND month = 7 AND day = 28 AND hour = 10 AND minute >= 15 AND minute <= 25;
-- Now onto phone calls,
SELECT * FROM phone_calls WHERE year = 2024 AND month = 7 AND day = 28 AND duration < 60;
-- Using some sub-query chenanigans, we narrowed the list of potential theives to 4 potential suspects,
SELECT * FROM people WHERE license_plate IN (SELECT license_plate FROM bakery_security_logs WHERE year = 2024 AND month = 7 AND day = 28 AND hour = 10 AND minute >= 15 AND minute <= 25) AND phone_number IN (SELECT caller FROM phone_calls WHERE year = 2024 AND month = 7 AND day = 28 AND duration < 60);
-- Saving that sub-query into a View to avoid having to type it each time.
CREATE VIEW suspects AS SELECT * FROM people WHERE license_plate IN (SELECT license_plate FROM bakery_security_logs WHERE year = 2024 AND month = 7 AND day = 28 AND hour = 10 AND minute >= 15 AND minute <= 25) AND phone_number IN (SELECT caller FROM phone_calls WHERE year = 2024 AND month = 7 AND day = 28 AND duration < 60);
-- Saving the flights sub-query.
CREATE VIEW potential_flights AS SELECT * FROM passengers WHERE passport_number IN (SELECT passport_number FROM suspects) AND flight_id IN (SELECT id FROM flights WHERE year = 2024 AND month = 7 AND day = 29);
-- A desperate attempt at lowering the suspects number.
SELECT * FROM suspects WHERE id IN (SELECT person_id FROM bank_accounts WHERE account_number in (SELECT account_number FROM atm_transactions WHERE year = 2024 AND month = 7 AND day = 28 AND transaction_type = 'withdraw'));
-- Checking earliest flight)
-- Made a view table, final_list
-- Finally found who it is, it's Bruce.
SELECT * FROM flights JOIN passengers ON passengers.flight_id = flights.id WHERE passengers.passport_number IN (SELECT passport_number FROM final_list);
-- To find the city.
| 36 | 8                 | 4                      | 2024 | 7     | 29  | 8    | 20     | 36        | 5773159633      | 4A  
