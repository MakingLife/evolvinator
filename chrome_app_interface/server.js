/**
 * Created by stephen on 29/04/15.
 */
/**
 * Created by stephen on 08/04/15.
 */

// module dependencies

var application_root = __dirname,
    express = require('express'),
    bodyParser = require('body-parser'),
    path = require('path');

var app = express();
var port = 6565;


// Config server
app.configure(function(){
    //parses request body and populates request.body
    app.use(express.bodyParser());

    //checks request.body for HTTP method overrides (something with PUT & POSTS)
    app.use(express.methodOverride());

    //perform route lookup based on url and HTTP method
    app.use(app.router);

    // where to serve static content
    app.use( express.static( path.join( application_root, '/') ) );

    // show all errors in development
    app.use(express.errorHandler({dumpExceptions: true, showStack: true }));
});

/* Nav routes */


/* API     -----    */
// define routes

// hello world

app.get('/', function(req, res){
    console.log("request = ");
    console.log(req);
    //console.log("response = "+res);
    res.send('running');
});


app.post('/', function(req, res){
    console.log("request = ");
    console.log(req);
    //console.log("response = "+res);
    //return(res.send(req));
});

app.listen(port, function(){
    console.log('Express server listening on port %d, public dir %s, in %s mode', port, application_root, app.settings.env);
    //console.log(path.join(__dirname, 'index.html'));
});