import os

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
    """Show portfolio of stocks"""

    portfolio = db.execute("SELECT symbol, sum(num_shares) AS shares FROM transactions WHERE person_id = ? GROUP BY symbol;", session["user_id"])
    portfolioClone = portfolio[:]

    # Remove 0 shares symbols
    for el in portfolioClone:
        if el["shares"] == 0:
            portfolio.remove(el)

    for el in portfolio:
        el["name"] = el["symbol"]
        el["shares"] = int(el["shares"])
        el["price"] = float(lookup(el["symbol"])["price"])
        el["totalToSumarize"] = el["shares"] * el["price"]
        el["total"] = usd(el["totalToSumarize"])
        el["price"] = usd(el["price"])

    cash = float((db.execute("SELECT cash FROM users WHERE id = ?;", session["user_id"]))[0]["cash"])

    total = cash
    for el in portfolio:
        total += el["totalToSumarize"]

    cash = usd(cash)
    total = usd(total)

    return render_template("index.html", portfolio=portfolio, cash=cash, total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":

        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        stackObject = lookup(symbol)

        try:
            shares = int(shares)
        except:
            return apology("wrong number of shares", 400)

        if shares < 0:
            return apology("wrong number of shares", 400)

        if not symbol or not stackObject:
            return apology("Invalid symbol", 400)
        if not shares:
            return apology("missing shares", 400)

        price = float(stackObject["price"])
        shares = shares
        totalCost = price * shares
        walletDB = db.execute("SELECT cash FROM users WHERE id = ?;", session["user_id"])
        wallet = float(walletDB[0]["cash"])

        if wallet < totalCost:
            return apology("not enough money", 400)

        db.execute("UPDATE users SET cash = ? WHERE id = ?;", (wallet - totalCost), session["user_id"])
        db.execute("INSERT INTO transactions (person_id, symbol, num_shares, price) VALUES (?, ?, ?, ?);", session["user_id"], symbol, shares, price)

        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    history = db.execute("SELECT symbol, num_shares, price, time FROM transactions WHERE person_id = ?;", session["user_id"])
    if not history:
        return apology("no history records yet", 400)

    for el in history:
        el["price"] =(usd(float(el["price"])))

    return render_template("history.html", history=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 400)

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
        stackObject = lookup(symbol)
        if stackObject == None:
            return apology("Wrong symbol!", 400)
        stackObject["price"] = usd(stackObject["price"])

        return render_template("quoted.html", stackObject=stackObject)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""


    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Check if there is already username in database
        usernameInDatabase = db.execute("SELECT COUNT(*) FROM users WHERE username = ?;", username)

        if not username or not usernameInDatabase[0]["COUNT(*)"] == 0:
            return apology("provide another username", 400)

        if not password or not(password == confirmation):
            return apology("provide password and confirm", 400)

        # Hash password and inserts data into database
        hashedPassword = generate_password_hash(password)
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?);", username, hashedPassword)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():

    """Sell shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        sharesToSell = int(request.form.get("shares"))

        if not symbol or not sharesToSell:
            return apology("provide symbol and amount of shares to sell", 400)

        price = float(lookup(symbol)["price"])
        sharesInWallet = int((db.execute("SELECT SUM(num_shares) FROM transactions WHERE symbol = ? AND person_id = ?;", symbol, session["user_id"]))[0]["SUM(num_shares)"])

        if sharesToSell > sharesInWallet:
            return apology("not enough shares in wallet", 400)

        cashInWallet = float((db.execute("SELECT cash FROM users WHERE id =?;", session["user_id"]))[0]["cash"])

        db.execute("UPDATE users SET cash = ? WHERE id = ?;", (cashInWallet + (price * sharesToSell)), session["user_id"])
        db.execute("INSERT INTO transactions (person_id, symbol, num_shares, price) VALUES (?, ?, ?, ?);", session["user_id"], symbol, -(sharesToSell), price)

        return redirect("/")

    else:
        shares = db.execute("SELECT symbol, sum(num_shares) AS shares FROM transactions WHERE person_id = ? GROUP BY symbol;", session["user_id"])
        sharesClone = shares[:]

        # Remove 0 shares symbols
        for share in sharesClone:
            if share["shares"] == 0:
                shares.remove(share)

        for share in shares:
            share["number"] = db.execute("SELECT SUM(num_shares) FROM transactions WHERE person_id = ? AND symbol = ?;", session["user_id"], share["symbol"])

    return render_template("sell.html", shares=shares)

@app.route("/password_change", methods=["GET", "POST"])
def password_change():
    """Change password"""


    if request.method == "POST":
        oldPassword = request.form.get("old_password")
        newPassword = request.form.get("new_password")
        confirmation = request.form.get("confirmation")

        if not oldPassword or not newPassword or not confirmation:
            return apology("fill all the blanks", 400)

        # Check if old password is correct
        userOldPasswordDB = db.execute("SELECT hash FROM users WHERE id = ?;", session["user_id"])

        if not check_password_hash(userOldPasswordDB[0]["hash"], oldPassword):
            return apology("wrong old password", 400)

        if not(newPassword == confirmation):
            return apology("new password and confirmation do not match", 400)

        # Hash password and inserts data into database
        hashedPassword = generate_password_hash(newPassword)
        db.execute("UPDATE users SET hash = ? WHERE id = ?;", hashedPassword, session["user_id"])

        return redirect("/")

    else:
        return render_template("password_change.html")


@app.route("/buy_share", methods=["GET", "POST"])
@login_required
def buyShare():
    """Buy shares of stock"""

    if request.method == "POST":
        symbol = request.form.get("symbol")
        sharesInWallet = db.execute("SELECT * FROM transactions WHERE symbol = ? AND person_id = ?;", symbol, session["user_id"])
        if not sharesInWallet:
            return redirect("/buy")
        return render_template("buy_share.html", symbol=symbol)
    else:
        return redirect("/buy")
