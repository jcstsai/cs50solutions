<?

    // require common code
    require_once("includes/common.php"); 
    
    // escape username and password for safety
    $username = mysqli_real_escape_string($connection, $_POST["username"]);
    $password = mysqli_real_escape_string($connection, $_POST["password"]);
    $password2 = mysqli_real_escape_string($connection, $_POST["password2"]);
    
    // check that passwords match
    if ($password !== $password2) {
        apologize("Passwords do not match!");
    }
    
    // check that username exists
    $sql = "SELECT uid FROM users WHERE username='$username'";
    $result = mysqli_query($connection, $sql);
    if (mysqli_num_rows($result) === 1) {
        apologize("Username already exists!");
    }
    
    // create user
    $sql = "
        INSERT INTO users (username, password, cash)
        VALUES ('$username', '$password', 10000.00)
    ";
    $result = mysqli_query($connection, $sql);
    if ($result === FALSE) {
        apologize("Unknown error with registration.");
    }
    
    // Get the user's UID and put it in session
    $_SESSION["uid"] = mysqli_insert_id($connection);

    // redirect to portfolio
    redirect("index.php");
?>
