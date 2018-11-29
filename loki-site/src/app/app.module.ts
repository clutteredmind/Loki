import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';

import { BsModalModule } from 'ng2-bs3-modal';

import { AppComponent } from './app.component';
import { NgbModule } from '@ng-bootstrap/ng-bootstrap';
import { AppRoutingModule } from './app-routing.module';
import { HomeComponent } from './components/pages/home/home.component';
import { SpinnerComponent } from './components/spinner/spinner.component';
import { SiteFooterComponent } from './components/site-footer/site-footer.component';
import { SiteHeaderComponent } from './components/site-header/site-header.component';
import { SiteSidebarComponent } from './components/site-sidebar/site-sidebar.component';
import { ScreenshotComponent } from './components/pages/screenshot/screenshot.component';
import { ErrorDisplayComponent } from './components/error-display/error-display.component';
import { DeviceListComponent } from './components/pages/device-list/device-list.component';
import { ProcessListComponent } from './components/pages/process-list/process-list.component';
import { SystemInformationComponent } from './components/pages/system-information/system-information.component';

@NgModule({
    declarations: [
        AppComponent,
        ErrorDisplayComponent,
        SiteFooterComponent,
        SiteHeaderComponent,
        SiteSidebarComponent,
        SpinnerComponent,
        HomeComponent,
        ProcessListComponent,
        DeviceListComponent,
        ScreenshotComponent,
        SystemInformationComponent
    ],
    imports: [
        NgbModule,
        BsModalModule,
        BrowserModule,
        AppRoutingModule
    ],
    providers: [
    ],
    bootstrap: [
        AppComponent
    ]
})
export class AppModule { }
