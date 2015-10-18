'use strict';

var path = require('path');
var chai = require('chai');
var expect = chai.expect;

chai.should();

describe('addon tests', function () {
	var base_addon_path = '../LokiAddons/x64/Debug/';

	describe('ProcessListAddon', function () {
		var addon_name = 'ProcessListAddon';
		var addon = null;
		
		beforeEach(function () {
			addon = require(path.join(base_addon_path, addon_name));
		});

		it('should exist', function () {
			addon.should.not.be.null;
		});
		
		it('should return a wrapped object with "new"', function () {
			var processListAddon = new addon.ProcessListAddon ();
			processListAddon.should.not.be.null;
		});
		
		describe('functions', function () {
			describe('getProcesses', function () {
				it('should have a "getProcesses" function', function () {
					var processListAddon = new addon.ProcessListAddon ();
					processListAddon.getProcesses().should.be.a.Function;
				});
				it('should return an array', function () {
					var processListAddon = new addon.ProcessListAddon ();
					(processListAddon.getProcesses()).should.be.an.Array;
				});
			});
		});
	});
	
	describe('DeviceListAddon', function () {
		var addon_name = 'DeviceListAddon';
		var addon = null;
		
		beforeEach(function () {
			addon = require(path.join(base_addon_path, addon_name));
		});

		it('should exist', function () {
			addon.should.not.be.null;
		});
		
		it('should return a wrapped object with "new"', function () {
			var deviceListAddon = new addon.DeviceListAddon ();
			deviceListAddon.should.not.be.null;
		});
		
		describe('functions', function () {
			describe('getDevices', function () {
				it('should have a "getDevices" function', function () {
					var deviceListAddon = new addon.DeviceListAddon ();
					deviceListAddon.getDevices().should.be.a.Function;
				});
				it('should return an array', function () {
					var deviceListAddon = new addon.DeviceListAddon ();
					(deviceListAddon.getDevices()).should.be.an.Array;
				});
			});
		});
	});
});
