<?
    // constants
    require_once("constants.php");

    // connect to database
    $connection = mysqli_connect(DB_SERVER, DB_USER, DB_PASS);

    // select database
    mysqli_select_db($connection, DB_NAME);

    // prepare an array for cities
    $cities = array();
    
    // ensure each parameter is in lat,lng format
    if (!preg_match("/^-?\d+(?:\.\d+)?,-?\d+(?:\.\d+)?$/", $_GET["sw"]) ||
        !preg_match("/^-?\d+(?:\.\d+)?,-?\d+(?:\.\d+)?$/", $_GET["ne"]))
    {
        header("Content-type: text/plain");
        print(json_encode($cities));
        exit;
    }
    
    // get southwest corner
    list($lat, $lng) = explode(",", $_GET["sw"]);

    $sw_lat = mysqli_real_escape_string($connection, $lat);
    $sw_lng = mysqli_real_escape_string($connection, $lng);
    
    // get northeast corner
    list($lat, $lng) = explode(",", $_GET["ne"]);
    $ne_lat = mysqli_real_escape_string($connection, $lat);
    $ne_lng = mysqli_real_escape_string($connection, $lng);

    // find <= 5 largest cities within view
    if ($sw_lng > $ne_lng)
    {
        $sql = "SELECT ZipCode, SUM(Population) AS p, ZipCode, Latitude, Longitude, State, City " .
               "FROM " . TBL_NAME . " " .
               "WHERE $sw_lat <= Latitude AND Latitude <= $ne_lat " .
               "AND ($sw_lng <= Longitude OR Longitude <= $ne_lng) " .
               "GROUP BY City, State ORDER BY p DESC LIMIT 5";
    }
    else
    {
        $sql = "SELECT ZipCode, SUM(Population) AS p, ZipCode, Latitude, Longitude, State, City " .
               "FROM " . TBL_NAME . " " .
               "WHERE $sw_lat <= Latitude AND Latitude <= $ne_lat " .
               "AND $sw_lng <= Longitude AND Longitude <= $ne_lng " .
               "GROUP BY City, State ORDER BY p DESC LIMIT 5";
    }
    $result = mysqli_query($connection, $sql);
            
    // iterate over result set
    while ($row = mysqli_fetch_assoc($result))
    {
        // prepare an array for city
        $city = array();

        // populate array with fields from database
        $city["ZipCode"] = $row["ZipCode"];
        $city["City"] = ucwords(strtolower($row["City"]));
        $city["State"] = $row["State"];
        $city["Latitude"] = $row["Latitude"];
        $city["Longitude"] = $row["Longitude"];

        // prepare array for city's articles
        $city["articles"] = array();

        // fetch city's articles
        if ($rss = @simplexml_load_file("http://news.google.com/news?geo={$row["ZipCode"]}&output=rss"))
        {
            foreach ($rss->channel->item as $item)
            {
                // prepare array for article
                $article = array();

                // populate array with data from RSS feed
                $article["link"] = (string) $item->link;
                $article["title"] = (string) $item->title;

                // associate article with city
                $city["articles"][] = $article;
            }
    
            // add city to array of cities
            $cities[] = $city;
        }
    }

    // output cities
    header("Content-type: text/plain");
    print(json_encode($cities));

?>
