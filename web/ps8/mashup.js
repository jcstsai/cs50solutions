/*
 * mashup.js
 *
 * Computer Science 50
 * Problem Set 8
 *
 * Implements mashup.
 */


// mashup's geocoder
var geocoder = null;

// map's home 
var home = new GLatLng(38.917221, -77.040307);

// mashup's Google Map
var map = null;


/*
 * void
 * addMarker(point, xhtml)
 *
 * Adds marker with given xhtml at given point on map.
 */

function addMarker(point, xhtml)
{
    // instantiate marker
    var marker = new GMarker(point);

    // prepare info window for city
    GEvent.addListener(marker, "click", function() {
        map.openInfoWindowHtml(point, xhtml);
    });

    // add marker to map
    map.addOverlay(marker);
}


/*
 * void
 * go()
 *
 * Pans map to desired location if possible, else yells at user.
 */

function go()
{
    var searchBoxContent;
    
    searchBoxContent = document.getElementById("q").value;
    geocoder.getLatLng(searchBoxContent, function(response) {
        map.setCenter(new GLatLng(response.lat(), response.lng()));
        home = new GLatLng(response.lat(), response.lng());

        // update markers
        update();
    });
}


/*
 * void
 * load()
 *
 * Loads (and configures) Google Map.
 */

function load() 
{
    // ensure browser supports Google Maps
    if (!GBrowserIsCompatible())
        return;

    // instantiate geocoder
    geocoder = new GClientGeocoder();

    // resize map's container
    resize();

    // instantiate map
    map = new GMap2(document.getElementById("map"), {
        zoomControl: true,
        mapTypeControl: true,
        scaleControl: true,
        streetViewControl: true,
        rotateControl: true,
        fullscreenControl: true
    });

    // center map on home
    map.setCenter(home, 13);

    // TODO: add control(s)

    // update markers anytime user drags or zooms map
    GEvent.addListener(map, "dragend", update);
    GEvent.addListener(map, "zoomend", update);

    // resize map anytime user resizes window
    GEvent.addDomListener(window, "resize", resize);

    // update markers
    update();

    // give focus to text field
    document.getElementById("q").focus();
}


/*
 * void
 * resize()
 *
 * Resizes map's container to fill area below form.
 */

function resize()
{
    // prepare to determine map's height
    var height = 0;

    // check for non-IE browsers
    if (window.innerHeight !== undefined)
        height += window.innerHeight;

    // check for IE
    else if (document.body.clientHeight !== undefined)
        height += document.body.clientHeight;

    // leave room for logo and form
    height -= 140;

    // maximize map's height if room
    if (height > 0)
    {
        // adjust height via CSS
        document.getElementById("map").style.height = height + "px";

        // ensure map exists
        if (map)
        {
            // resize map
            map.checkResize();

            // update markers
            update();
        }
    }
}


/*
 * void
 * unload()
 *
 * Unloads Google Map.
 */

function unload()
{
    // unload Google's API
    GUnload();
}


/*
 * void
 * update()
 *
 * Updates map with markers for largest <= 5 cities within view.
 * Also displays marker for home if within view.
 */

function update()
{
    var xhr,
        url;
    
    // clear any existing markers
    map.clearOverlays();

    // get map's bounds
    var bounds = map.getBounds();
    
    // query for the five cities in the view
    // instantiate XMLHttpRequest object
    try {
        xhr = new XMLHttpRequest();
    } catch (e) {
        xhr = new ActiveXObject("Microsoft.XMLHTTP");
    }
    // handle old browsers
    if (xhr === null) {
        alert("Ajax not supported by your browser!");
        return;
    }
    
    url = "proxy.php?sw=" +
        bounds.getSouthWest().lat() + "," + bounds.getSouthWest().lng() +
        "&ne=" +
        bounds.getNorthEast().lat() + "," + bounds.getNorthEast().lng();

    xhr.onreadystatechange = function() {
        var infos,
            xhtml,
            i,
            articles,
            j;
        
        if (xhr.readyState === 4) {
            document.getElementById("progress").style.display = "none";
            if (xhr.status === 200) {
                infos = eval("(" + xhr.responseText + ")");
                for (i = 0; i < infos.length; i++) {
                    articles = "";
                    for (j = 0; j < infos[i].articles.length; j++) {
                        articles += ("<li>" + 
                            "<a href='" + infos[i].articles[j].link + "'>" +
                                infos[i].articles[j].title +
                            "</a>" +
                        "</li>");
                    }
                    addMarker(
                        new GLatLng(infos[i].Latitude, infos[i].Longitude),
                        "<b>" + infos[i].City + ",  " + infos[i].State + "</b>" +
                        "<br />" +
                        "<ul>" +
                            articles +
                        "</ul>"
                    );
                }
            } else {
                alert("Error with Ajax call!");
            }
        }
    };
    xhr.open("GET", url, true);
    xhr.send(null);
    document.getElementById("progress").style.display = "block";

    // mark home if within bounds
    if (bounds.containsLatLng(home))
    {
        // prepare XHTML
        var xhtml = "<b>Home Sweet Home</b>";

        // add marker to map
        addMarker(home, xhtml);
    }

    // TODO: contact proxy, add markers
}
