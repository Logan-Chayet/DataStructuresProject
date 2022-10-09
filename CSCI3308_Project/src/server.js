var express = require('express'); //Ensure our express framework has been added
var app = express();
var bodyParser = require('body-parser'); //Ensure our body-parser tool has been added
app.use(bodyParser.json());              // support json encoded bodies
app.use(bodyParser.urlencoded({ extended: true })); // support encoded bodies
const axios = require('axios');
const qs = require('query-string');
//Create Database Connection
var pgp = require('pg-promise')();

const DB_dbConfig = {
	host: 'db',
	port: 5432,
	database: 'OMDb_db',
	user: 'postgres',
	password: 'pwd'
};

const isProduction = process.env.NODE_ENV === 'production';
const dbConfig = isProduction ? process.env.DATABASE_URL : DB_dbConfig;
if (isProduction) {
	pgp.pg.defaults.ssl = {rejectUnauthorized: false};
  }
  
  const db = pgp(dbConfig);
//app.listen(3000);
const PORT = process.env.PORT || 8080;

const server = app.listen(PORT, () => {
	console.log(`Express running → PORT ${server.address().port}`);
});

  // set the view engine to ejs
  app.set('view engine', 'ejs');
  app.set('views', __dirname + '/views');
  app.use(express.static(__dirname + '/'));//This line is necessary for us to use relative paths and access our resources directory  

app.get('/', function(req, res) {
	res.render('pages/main', {
	  my_title: "OMDb Search",
	  items: '',
	  error: false,
	  message: ''
	});
  });

app.get('/main', function(req, res) {
	res.render('pages/main', {
	  my_title: "OMDb Search",
	  items: '',
	  error: false,
	  message: ''
	});
  });

  app.post('/get_feed', function(req, res) {
	var title = req.body.title; 
	var api_key = "4b1a2551"; 
  
	if(title) {
	  axios({
		url: `http://www.omdbapi.com/?t=${title}&apikey=${api_key}&`,
		  method: 'GET',
		  dataType:'json',
		})
		  .then(items => {
	
			console.log(items.data)
			console.log(items.data.Title)
			// for(var i=0; i < items.data.Ratings.length; i++) {
			//   console.log(items.data.Ratings[i]);
			// }
			// if(!items.data.Title){
			// 	throw Error("-")
			// }
			res.render('pages/main',{
			  my_title: "OMDb Movie",
			  items: items.data,
			  error: false,
			  message: ''
			  
			})
		  })
		  .catch(error => {
			if (error.response) {
			  console.log(error.response.data);
			  console.log(error.response.status);
			}
			console.log(error);
			res.render('pages/main',{
			  my_title: "OMDb Movie",
			  items: '',
			  error: true,
			  message: error
			  
			})
		  });
  
  
	}
	else {
	  // TODO: Render the home page and include an error message (e.g., res.render(...);); Why was there an error? When does this code get executed? Look at the if statement above
	  // Stuck? On the web page, try submitting a search query without a search term
	  res.render('pages/main',{
		my_title: title+" Reviews",
		items: '',
		error: true,
		message: 'Search field cannot be empty'
	  })
	}
  });
  app.get('/reviews', function(req, res){
	// var review_date = req.query.review_date;
	// var movie_name = req.query.movie_name;
	// var movie_review = req.query.movie_review;
	var query1 = 'SELECT * FROM reviews;';
	console.log(query1)
	db.task('get-everything', task => {
		return task.batch([
			task.any(query1),
		]);
	})
	.then(items => {
		console.log(items)
		res.render('pages/reviews',{
				my_title: "See Reviews",
				items: items[0],
				// movie_name: movie_name,
				// movie_review: movie_review,
				// review_date: review_date
			})
	})
	.catch(error => {
		// display error message in case an error
			console.log('error', error);
			res.render('pages/reviews',{
				my_title: "See Reviews",
				items: '',
				error: true,
				message: error
			})
	});
});
app.post('/addreviews', function(req, res) {
	var review_date = new Date();
	var movie_name = req.body.movie_name;
	var movie_review = req.body.movie_review;
	var insert_statement = "INSERT INTO reviews (movie_name, movie_review, review_date) VALUES('"+movie_name+"', '"+movie_review+"', '"+review_date+"');";// Write a SQL statement to insert a color into the favorite_colors table
	var review_select = "SELECT * FROM reviews;";// Write a SQL statement to retrieve all of the colors in the favorite_colors table
	console.log()
	db.task('get-everything', task => {
        return task.batch([
            task.any(insert_statement),
            task.any(review_select)
        ]);
    })
    .then(items => {
    	res.render('pages/reviews',{
				my_title: "Reviews",
				items: items[1],
				movie_review: movie_review// Return the color message of the color added to the table
			})
    })
    .catch(error => {
            console.log('error', error);
            res.render('pages/main', {
				my_title: 'Home Page',
				items: '',
				movie_review: '',
				error: true,
				message: error
            })
    });
});
app.get('/searchReviews', function(req, res){
	var review_date = req.query.review_date;
	var movie_name = req.query.title;
	var movie_review = req.query.movie_review;
	var searchQuery = `SELECT * FROM reviews WHERE movie_name='${movie_name}';`;
	// var searchQuery = "SELECT * FROM reviews WHERE movie_name='Batman';";
	console.log(searchQuery)
	db.task('get-everything', task => {
		return task.batch([
			task.any(searchQuery)
		]);
	})
	.then(items => {
		console.log(items)
		res.render('pages/reviews',{
				my_title: "See Reviews",
				items: items[0],
				movie_name: movie_name,
				movie_review: movie_review,
				review_date: review_date
			})
	})
	.catch(error => {
		// display error message in case an error
			console.log('error', error);
			res.render('pages/reviews',{
				my_title: "See Reviews",
				items: '',
				error: true,
				message: error
			})
	});
});
  
  app.listen(3000);
  console.log('3000 is the magic port');