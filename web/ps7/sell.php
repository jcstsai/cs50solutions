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
    <title>C$50 Finance: Sell</title>
  </head>

  <body>

    <div align="center">
      <a href="index.php"><img alt="C$50 Finance" border="0" src="images/logo.gif" /></a>
    </div>

    <div align="center">
      <form action="sell2.php" method="post">
        <select name="symbol">
            <option selected disabled>Symbol</option>
            <?
                $uid = $_SESSION["uid"];
                $sql = "SELECT symbol FROM stocks WHERE uid = $uid";
                $result = mysqli_query($connection, $sql);

                if (mysqli_num_rows($result) === 0) {
                    apologize("Nothing to sell!");
                } else {
                    while ($row = mysqli_fetch_array($result)) { ?>
                        <option><?= $row["symbol"] ?></option>
                    <? }
                }
            ?>
        </select>
        <div style="margin: 10px;">
          <input type="submit" value="Sell" />
        </div>
      </form>
    </div>

  </body>

</html>
