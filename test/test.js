'use strict';

require = require('really-need');
var request = require('supertest');
var chai = require('chai');
chai.use(require('chai-string'));
var expect = chai.expect;

describe('base server functionality', function (){
	var server;
	beforeEach(function (){
		server = require('../app.js', { bustCache: true });
	});
	
	it('responds to /', function (done){
		request(server)
			.get('/')
			.expect(200, done);
	});
	
	it('gives a 404 for everything else', function (done){
		request(server)
			.get('/everything/else')
			.expect(404, done);
	});
});

describe('I\'m showing sasha how to write tests', function(){
	var blah = 'hello';
	it('will be equal to hello', function (){
		expect(blah).to.be.reverseOf('olleh');
	});
});
