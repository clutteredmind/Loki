import { NgModule } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';

import { AppRoutingModule } from './app-routing.module';

import { AppComponent } from './app.component';
import { DeviceListComponent } from './components/device-list/device-list.component';
import { FooterComponent } from './components/footer/footer.component';
import { HeaderComponent } from './components/header/header.component';
import { HomeComponent } from './components/home/home.component';
import { ProcessListComponent } from './components/process-list/process-list.component';
import { ScreenshotComponent } from './components/screenshot/screenshot.component';
import { SidebarComponent } from './components/sidebar/sidebar.component';
import { SystemInformationComponent } from './components/system-information/system-information.component';
import { NgbModule } from '@ng-bootstrap/ng-bootstrap';

@NgModule({
  declarations: [
    AppComponent,
    DeviceListComponent,
    FooterComponent,
    HeaderComponent,
    HomeComponent,
    ProcessListComponent,
    ScreenshotComponent,
    SidebarComponent,
    SystemInformationComponent
  ],
  imports: [
    AppRoutingModule,
    BrowserModule,
    NgbModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
