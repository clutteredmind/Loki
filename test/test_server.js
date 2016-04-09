'use strict';

require = require('really-need');

var uuid = require('node-uuid');
var request = require('supertest');
var chai = require('chai');

chai.use(require('chai-string'));
var expect = chai.expect;

describe('base server functionality', function () {
	var server;

	beforeEach(function () {
		server = require('../app.js', { bustCache: true });
	});

	it('responds to /', function (done) {
		this.slow(2000);
		request(server).get('/').expect(200, done);
	});

	it('returns a 404 response code for invalid URLs', function (done) {
		// use node-uuid to generate a basically-random URL that will fail
		request(server).get('/' + uuid.v1()).expect(404, done);
	});
});
