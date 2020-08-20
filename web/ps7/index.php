<?

    // require common code
    require_once("includes/common.php"); 

?>

<!DOCTYPE html 
     PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
     "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml">

  <head>
    <link href="css/styles.css" rel="stylesheet" type="text/css" />
    <title>C$50 Finance: Home</title>
  </head>

  <body>

    <div align="center">
      <a href="index.php"><img alt="C$50 Finance" border="0" height="110" src="images/logo.gif" width="544"/></a>
    </div>
    
    <div align="center">
        <table border="0">
            <tr>
                <td style="padding: 5px;"><a href="quote.php">Quote</a></td>
                <td style="padding: 5px;"><a href="buy.php">Buy</a></td>
                <td style="padding: 5px;"><a href="sell.php">Sell</a></td>
                <td style="padding: 5px;"><a href="history.php">History</a></td>
                <td style="padding: 5px;"><a href="changepassword.php">Change Password</a></td>
            </tr>
        </table>
    </div>
      
    <br />

    <div align="center">
        <table border="1">
            <tr>
                <td><b>Symbol</b></td>
                <td><b>Name</b></td>
                <td><b>Shares</b></td>
                <td><b>Price</b></td>
                <td><b>TOTAL</b></td>
            </tr>
            <? 
                $uid = $_SESSION["uid"];
                
                $result = mysqli_query($connection, "SELECT cash FROM users WHERE uid=$uid");
                $row = mysqli_fetch_array($result);
                ?>
                    <tr>
                        <td>CASH</td>
                        <td></td>
                        <td></td>
                        <td></td>
                        <td><?= '$' . round($row["cash"], 2) ?></td>
                    </tr>
                <?
                
                $result = mysqli_query($connection, "SELECT symbol, shares FROM stocks WHERE uid=$uid");
                while ($row = mysqli_fetch_array($result)) {
                    $s = lookup($row["symbol"]); ?>
                    <tr>
                        <td><?= $row["symbol"] ?></td>
                        <td><?= $s->name ?></td>
                        <td><?= $row["shares"] ?></td> 
                        <td><?= '$' . $s->price ?></td>
                        <td><?= '$' . ($s->price * $row["shares"]) ?></td>
                    </tr>
                <? } ?>
        </table>
    </div>

    <div align="center" style="margin: 10px;">
      <a href="logout.php">log out</a>
    </div>

  </body>

</html>
