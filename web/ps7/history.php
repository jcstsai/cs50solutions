<?
    // require common code
    require_once("includes/common.php"); 
    
    // read transactions
    $uid = $_SESSION["uid"];
    $sql = "
        SELECT type, datetime, symbol, shares, price
        FROM transactions
        WHERE uid = $uid
        ORDER BY datetime DESC
    ";
    $result = mysqli_query($connection, $sql);
    

    
    // output as HTML
    ?>
    <!DOCTYPE html 
        PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
        "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

    <html xmlns="http://www.w3.org/1999/xhtml">

    <head>
        <link href="css/styles.css" rel="stylesheet" type="text/css" />
        <title>C$50 Finance: History</title>
    </head>

    <body>

    <div align="center">
        <a href="index.php"><img alt="C$50 Finance" border="0" height="110" src="images/logo.gif" width="544"/></a>
    </div>

    <div align="center">
        <table border="1">
            <tr>
                <td><b>Transaction</b></td>
                <td><b>Date/Time</b></td>
                <td><b>Symbol</b></td>
                <td><b>Shares</b></td>
                <td><b>Price</b></td>
            </tr>
            <? while ($row = mysqli_fetch_array($result)) { ?>
                <tr>
                    <td><?= $row["type"] ?></td>
                    <td><?= $row["datetime"] ?></td>
                    <td><?= $row["symbol"] ?></td> 
                    <td><?= $row["shares"] ?></td>
                    <td><?= '$' . round($row["price"], 2) ?></td>
                </tr>
            <? } ?>
         </table>
    </div>
    </body>
    </html>
    <?
?>
