<?

    // require common code
    require_once("includes/common.php"); 
        
    // look up stock
    $stock = lookup($_POST["symbol"]);

    if ($stock === NULL) {
        apologize("Invalid stock symbol!");
    } else {
        // get number of shares
        $uid = $_SESSION["uid"];
        $symbol = mysqli_real_escape_string($connection, $_POST["symbol"]);
        $sql = 
            "SELECT shares FROM stocks WHERE uid = $uid AND symbol = '$symbol'";
        $result = mysqli_query($connection, $sql);
        $shares = mysqli_fetch_array($result)["shares"];
            
        // remove stocks
        $sql = "DELETE FROM stocks WHERE uid = $uid AND symbol = '$symbol'";
        mysqli_query($connection, $sql);
        
        // add money
        $price = $stock->price;
        $amount = $price * $shares;
        $sql = "UPDATE users SET cash = cash + $amount WHERE uid = $uid";
        mysqli_query($connection, $sql);
        
        // record transaction
        $sql = "
            INSERT INTO transactions (uid, type, symbol, shares, price, datetime)
            VALUES ($uid, 'SELL', '$symbol', $shares, $price, NOW())
        ";
        mysqli_query($connection, $sql);
    }
    
    // redirect to portfolio
    redirect("index.php");
?>