<?

    // require common code
    require_once("includes/common.php"); 
        
    // check for blank symbol
    if ($_POST["symbol"] === "") {
        apologize("Please enter a symbol.");
    }
    
    // check for positive integer shares
    if (!preg_match("/^\d+$/", $_POST["shares"])) {
        apologize("Please enter a positive integer number of shares.");
    }
    
    // look up stock
    $stock = lookup($_POST["symbol"]);
    if ($stock === NULL) {
        apologize("Invalid stock symbol!");
    }
    
    $uid = $_SESSION["uid"];
    $symbol = mysqli_real_escape_string($connection, $_POST["symbol"]);
    $shares = intval($_POST["shares"]);
    
    // get the price of these shares and check if user has enough money
    $price = $stock->price;
    $amount = $price * $shares;
    $sql = "SELECT cash FROM users WHERE uid = $uid";
    $result = mysqli_query($connection, $sql);
    $cash = mysqli_fetch_array($result)["cash"];
    if ($cash < $amount) {
        apologize("Not enough cash!");
    }
    
    // withdraw money
    $sql = "UPDATE users SET cash = cash - $amount WHERE uid = $uid";
    mysqli_query($connection, $sql);
    
    // add shares
    $sql = "
        INSERT INTO stocks (uid, symbol, shares)
        VALUES ($uid, '$symbol', $shares)
        ON DUPLICATE KEY UPDATE shares = shares + VALUES (shares)
    ";
    mysqli_query($connection, $sql);
    
    // record transaction
    $sql = "
        INSERT INTO transactions (uid, type, symbol, shares, price, datetime)
        VALUES ($uid, 'BUY', '$symbol', $shares, $price, NOW())
    ";
    mysqli_query($connection, $sql);

    // redirect to portfolio
    redirect("index.php");
?>