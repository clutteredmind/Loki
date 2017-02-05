import { NgModule }                 from '@angular/core';
import { BrowserModule }            from '@angular/platform-browser';

import { AppComponent }             from './app.component';

import { routing }                  from './app.routing';

import { NavbarComponent }          from './components/navbar/navbar.component';
import { ErrorDisplayComponent }    from './components/error-display/error-display.component';
import { SpinnerComponent }         from './components/spinner/spinner.component';
import { DeviceListComponent }      from './components/pages/device-list/device-list.component';
import { HomeComponent }            from './components/pages/home/home.component';
import { ProcessListComponent }     from './components/pages/process-list/process-list.component';
import { ScreenshotComponent }      from './components/pages/screenshot/screenshot.component';

import { SocketService }            from './services/socket.service';

@NgModule({
    imports: [
        BrowserModule,
        routing
    ],
    declarations: [
        AppComponent,
        NavbarComponent,
        SpinnerComponent,
        HomeComponent,
        ErrorDisplayComponent,
        DeviceListComponent,
        ProcessListComponent,
        ScreenshotComponent
    ],
    bootstrap: [
        AppComponent
    ],
    providers: [
        SocketService
    ]
})
export class AppModule {

}
