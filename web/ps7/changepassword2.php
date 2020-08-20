<?

    // require common code
    require_once("includes/common.php"); 
    
    // escape username and password for safety
    $password = mysqli_real_escape_string($connection, $_POST["password"]);
    $password2 = mysqli_real_escape_string($connection, $_POST["password2"]);
    
    // check that passwords match
    if ($password !== $password2) {
        apologize("Passwords do not match!");
    }
    
    // check if password is not blank
    if ($password === "") {
        apologize("Password cannot be blank!");
    }
    
    // update password
    $uid = $_SESSION["uid"];
    $sql = "
        UPDATE users
        SET password = '$password'
        WHERE uid = $uid
    ";
    $result = mysqli_query($connection, $sql);
    if ($result === FALSE) {
        apologize("Unknown error with password change.");
    }
    
    // redirect to portfolio
    redirect("index.php");
?>
