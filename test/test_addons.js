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
			var process_list_addon = new addon.ProcessListAddon ();
			process_list_addon.should.not.be.null;
		});
		
		describe('functions', function () {
			var process_list_addon;
			beforeEach(function () {
				process_list_addon = new addon.ProcessListAddon ();
			});
			
			describe('getProcesses', function () {
				it('should be a function', function () {
					process_list_addon.getProcesses.should.be.a.Function;
				});
				it('should return an array', function () {
					(process_list_addon.getProcesses()).should.be.an.Array;
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
			var device_list_addon = new addon.DeviceListAddon ();
			device_list_addon.should.not.be.null;
		});
		
		describe('functions', function () {
			var device_list_addon;
			beforeEach(function () {
				device_list_addon = new addon.DeviceListAddon();
			});
			describe('getDevices', function () {
				it('should be a function', function () {
					device_list_addon.getDevices.should.be.a.Function;
				});
				it('should return an array', function () {
					(device_list_addon.getDevices()).should.be.an.Array;
				});
			});
		});
	});
	
	describe('ScreenshotAddon', function () {
		var addon_name = 'ScreenshotAddon';
		var addon = null;
		
		beforeEach(function () {
			addon = require(path.join(base_addon_path, addon_name));
		});
		
		it('should exist', function () {
			addon.should.not.be.null;
		});
		
		it('should return a wrapped object with "new"', function () {
			var screenshot_addon = new addon.ScreenshotAddon();
			screenshot_addon.should.not.be.null;
		});
		
		describe('functions', function () {
			var screenshot_addon;
			beforeEach(function () {
				screenshot_addon = new addon.ScreenshotAddon();
			});
			describe('captureScreen', function () {
				it('should be a function', function () {
					screenshot_addon.captureScreen.should.be.a.Function;
				});
				it('should throw an exception if called with no arguments', function () {
					expect(screenshot_addon.captureScreen.bind(screenshot_addon)).to.throw("captureScreen: Invalid parameter count. Expected: Parameter 0 'callback' - function");
				});
				it('should throw an exception if called with an argument that is not a function', function () {
					expect(screenshot_addon.captureScreen.bind(screenshot_addon, true)).to.throw("captureScreen: Parameter 0 'callback' must be a function");
				});
				it('should return "undefined" when called with a callback', function () {
					expect(screenshot_addon.captureScreen(function () {})).to.be.Undefined;
				});
			});
		});
	});
});
