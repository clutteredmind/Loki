/// <reference path="../node_modules/@types/node/index.d.ts" />
/// <reference path="../node_modules/@types/jasmine/index.d.ts" />

import { platformBrowserDynamic }   from '@angular/platform-browser-dynamic';
import { enableProdMode }           from '@angular/core';

import { AppModule }                from './app.module';

//enableProdMode();
platformBrowserDynamic().bootstrapModule(AppModule);
