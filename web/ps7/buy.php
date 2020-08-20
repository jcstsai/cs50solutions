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
    <title>C$50 Finance: Buy</title>
  </head>

  <body>
    <div align="center">
        <a href="index.php"><img alt="C$50 Finance" border="0" src="images/logo.gif" /></a>
    </div>

    <div align="center">
        <form action="buy2.php" method="post">
            <table border="1">
                <tr>
                    <td class="field">Symbol:</td>
                    <td><input name="symbol" type="text" /></td>
                </tr>
                <tr>
                    <td class="field">Shares:</td>
                    <td><input name="shares" type="shares" /></td>
                </tr>
            </table>
            <div style="margin: 10px;">
                <input type="submit" value="Buy" />
            </div>
        </form>
    </div>

  </body>

</html>
