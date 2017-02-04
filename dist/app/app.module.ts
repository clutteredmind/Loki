import { NgModule }                 from '@angular/core';
import { BrowserModule }            from '@angular/platform-browser';

import { AppComponent }             from './app.component';

import { routing }                  from './app.routing';

import { NavbarComponent }          from './components/navbar/navbar.component';
import { DeviceListComponent }      from './components/pages/device-list/device-list.component';
import { HomeComponent }            from './components/pages/home/home.component';
import { ProcessListComponent }     from './components/pages/process-list/process-list.component';
import { ScreenshotComponent }      from './components/pages/screenshot/screenshot.component';

@NgModule({
    imports: [
        BrowserModule,
        routing
    ],
    declarations: [
        AppComponent,
        NavbarComponent,
        HomeComponent,
        DeviceListComponent,
        ProcessListComponent,
        ScreenshotComponent
    ],
    bootstrap: [
        AppComponent
    ],
    providers: [
    ]
})
export class AppModule {

}
