const moment = require ("moment"); 
const request = require ("request"); 
"use strict";
const uri = "https://api.openweathermap.org/data/2.5/weather";
const qs = {
  id: "4927510",
  appid: "32843bad9e96bb36c7935458544b1628",
  units: "imperial",
};

function reportWeather(result){
	console.log(`On ${moment()}, it's ${result.main.temp} degress.`);
}

function reportError(error) {
	console.log(`Something went haywire: ${error}`);
}

function requestPromisified(uri, qs) { 
	return new Promise ((resolve, reject) => {
		request ({uri: uri, qs:qs}, (error, response, body) => {
			const json = JSON.parse(body);
        		if (error) {
                                return reject(err);
                        }else{
                                return resolve(json);
                        }
		});
	});	

} 

function getTimeAndTempPromise(uri, qs) { 
	requestPromisified(uri, qs) 
		.then(content => reportWeather(content))
		.catch(err => reportError(err));
} 

async function getTimeAndTempAsync(uri, qs){
	try {
	let data = await requestPromisified(uri, qs);
	reportWeather(data);	
	} catch (err){ 
	 	reportError(err);
	} 
}

getTimeAndTempPromise(uri, qs);
getTimeAndTempAsync(uri, qs);
