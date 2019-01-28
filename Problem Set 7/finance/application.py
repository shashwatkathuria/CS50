import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached


@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    cash = db.execute("SELECT * FROM users WHERE id=:idd", idd=session["user_id"])
    rows = db.execute("SELECT * FROM portfolio where id=:idd", idd=session["user_id"])
    if rows == []:
        flash("Portfolio Empty!!")
        return render_template("noportfolio.html", cash=usd(cash[0]["cash"]))
    else:
        return render_template("portfolio.html", rows=rows, cash=usd(cash[0]["cash"]))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        try:
            shares = int(request.form.get("shares"))
        except:
            return apology("error", 400)
        if not request.form.get("symbol"):
            return apology("must enter a stock quote", 400)
        elif (not request.form.get("shares")) or int(request.form.get("shares")) <= 0:
            return apology("must enter the number of stocks to buy", 400)
        else:
            quote = lookup(request.form.get("symbol"))
            if quote == None:
                return apology("must enter a valid stock quote", 400)
        noofquotes = float(request.form.get("shares"))
        quoteprice = quote["price"]
        total = noofquotes * quoteprice
        row = db.execute("SELECT * FROM users WHERE id=:t", t=session["user_id"])
        available = row[0]["cash"]
        if (available - total) < 0:
            return apology("balance limit exceeded", 400)
        else:
            available -= total
            db.execute("UPDATE users SET cash=:t WHERE id=:z", t=available, z=session["user_id"])
        #    update portfolio information with share quote and no of shares
            db.execute("INSERT INTO transactions (id,symbol,noofshares,pricepershare,totalprice,type) VALUES (:i,:s,:n,:pps,:tp,'bought')",
                       i=session["user_id"], s=request.form.get("symbol"), n=request.form.get("shares"), pps=quoteprice, tp=total)
            w = db.execute("SELECT * FROM portfolio WHERE id=:i AND symbol=:y", i=session["user_id"], y=request.form.get("symbol"))
            if w == []:
                db.execute("INSERT INTO portfolio (id,symbol,noofshares) VALUES (:i,:s,:n)",
                           i=session["user_id"], s=request.form.get("symbol"), n=request.form.get("shares"))

            else:
                noofstocks = w[0]["noofshares"]
                noofstocks += int(request.form.get("shares"))
                db.execute("UPDATE portfolio SET noofshares=:t WHERE id=:i and symbol=:p",
                           t=noofstocks, i=session["user_id"], p=request.form.get("symbol"))
        flash("Bought!")
        flash(usd(quoteprice))
        flash(usd(available))
        flash(usd(total))
        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    rows = db.execute("SELECT * FROM transactions WHERE id=:idd ORDER BY time DESC", idd=session["user_id"])
    if rows == []:
        flash("No Transactions Have Taken Place!!")
        return redirect("/history")
    else:
        return render_template("history.html", rows=rows)


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
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
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
        if not request.form.get("symbol"):
            return apology("must enter a stock quote", 400)
        else:
            quote = lookup(request.form.get("symbol"))

        if quote == None:
            return apology("must enter a valid stock quote", 400)
        return render_template("quoted.html", quote=quote["symbol"], price=usd(quote["price"]))
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":
        if not request.form.get("username"):
            return apology("must provide a username", 400)

        elif not request.form.get("password"):
            return apology("must provide a password", 400)

        elif not request.form.get("confirmation"):
            return apology("must confirm the password", 400)

        elif not (request.form.get("password") == request.form.get("confirmation")):
            # flash(request.form.get("password"))
            # flash(request.form.get("confirmation"))
            return apology("passwords do not match", 400)
    #    print("HELLLLLOOOOOOOOO")
        presentornot = db.execute("SELECT * FROM users WHERE username=:u", u=request.form.get("username"))
    #    print(presentornot)
        if len(presentornot) == 0:
            new_user_id = db.execute("INSERT INTO users (username,hash) VALUES (:username,:hash1)",
                                     username=request.form.get("username"), hash1=generate_password_hash(request.form.get("password")))
            session["user_id"] = new_user_id
            rows = db.execute("SELECT * FROM users")
    #        print(rows)
            flash("Registered!")
            return render_template("login.html")
        else:
            flash("Username already taken")
            return apology("username already taken", 400)

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        try:
            shares = request.form.get("shares")
        except:
            return apology("error", 400)
        if not request.form.get("symbol"):
            return apology("must provide a stock quote", 400)

        elif (not request.form.get("symbol")) or int(request.form.get("shares")) <= 0:
            return apology("must provide the number of stocks to sell", 400)

        else:
            quote = lookup(request.form.get("symbol"))
            if quote == None:
                return apology("must provide a valid stock quote", 400)

        noofquotes = float(request.form.get("shares"))
        quoteprice = quote["price"]
        total = noofquotes * quoteprice
        row = db.execute("SELECT * FROM users WHERE id=:idd", idd=session["user_id"])
        available = row[0]["cash"]
        transac = db.execute("SELECT * FROM transactions WHERE id=:idd", idd=session["user_id"])
        noofavailable = transac[0]["noofshares"]

        if noofavailable >= noofquotes:
            w = db.execute("SELECT * FROM portfolio WHERE id=:idd AND symbol=:sym",
                           idd=session["user_id"], sym=request.form.get("symbol"))
            if w == []:
                return apology("stock not in your portfolio", 400)
            else:
                toupdate = w[0]["noofshares"]
                toupdate -= int(request.form.get("shares"))
                db.execute("UPDATE portfolio SET noofshares=:y WHERE id=:idd AND symbol=:sym",
                           y=toupdate, idd=session["user_id"], sym=request.form.get("symbol"))
            flash("Sold")
            flash(usd(available + total))
            flash(usd(quoteprice))
            flash(usd(total))
            db.execute("UPDATE users SET cash=:t WHERE id=:z", t=(available + total), z=session["user_id"])
            db.execute("INSERT INTO transactions (id,symbol,noofshares,pricepershare,totalprice,type) VALUES (:idd,:sym,:n,:pps,:tp,'sold')",
                       idd=session["user_id"], sym=request.form.get("symbol"), n=request.form.get("shares"), pps=quoteprice, tp=total)
            return redirect("/sell")
        else:
            return apology("number of stocks available exceeded", 400)
    else:
        stocks = db.execute(
            "SELECT symbol, noofshares as total_shares FROM portfolio WHERE id = :user_id GROUP BY symbol HAVING noofshares > 0", user_id=session["user_id"])

        return render_template("sell.html", stocks=stocks)


def errorhandler(e):
    """Handle error"""
    return apology(e.name, e.code)


# listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
