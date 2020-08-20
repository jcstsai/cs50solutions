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
    <title>C$50 Finance: Change Password</title>
  </head>

  <body>

    <div align="center">
      <a href="index.php"><img alt="C$50 Finance" border="0" src="images/logo.gif" /></a>
    </div>

    <div align="center">
      <form action="changepassword2.php" method="post">
        <table border="1">
          <tr>
            <td class="field">New Password:</td>
            <td><input name="password" type="password" /></td>
          </tr>
          <tr>
            <td class="field">New Password (again):</td>
            <td><input name="password2" type="password" /></td>
          </tr>
        </table>
        <div style="margin: 10px;">
          <input type="submit" value="Change Password" />
        </div>
      </form>
    </div>

  </body>

</html>
