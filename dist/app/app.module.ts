import { NgModule }                     from '@angular/core';
import { BrowserModule }                from '@angular/platform-browser';

import { AppComponent }                 from './app.component';

import { routing }                      from './app.routing';

import { Ng2Bs3ModalModule }            from 'ng2-bs3-modal/ng2-bs3-modal';

import { NavbarComponent }              from './components/navbar/navbar.component';
import { ErrorDisplayComponent }        from './components/error-display/error-display.component';
import { SpinnerComponent }             from './components/spinner/spinner.component';
import { DeviceListComponent }          from './components/pages/device-list/device-list.component';
import { HomeComponent }                from './components/pages/home/home.component';
import { ProcessListComponent }         from './components/pages/process-list/process-list.component';
import { ScreenshotComponent }          from './components/pages/screenshot/screenshot.component';
import { SystemInformationComponent }   from './components/pages/system-information/system-information.component';

import { SocketService }                from './services/socket.service';

@NgModule({
    imports: [
        BrowserModule,
        routing,
        Ng2Bs3ModalModule
    ],
    declarations: [
        AppComponent,
        NavbarComponent,
        SpinnerComponent,
        HomeComponent,
        ErrorDisplayComponent,
        DeviceListComponent,
        ProcessListComponent,
        ScreenshotComponent,
        SystemInformationComponent
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
