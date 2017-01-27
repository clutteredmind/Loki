import { BrowserModule }    from '@angular/platform-browser';
import { NgModule }         from '@angular/core';
import { FormsModule }      from '@angular/forms';
import { HttpModule }       from '@angular/http';
import { RouterModule }     from '@angular/router';

import { AppComponent } from './app.component';

import { AppRoutingModule } from './app-routing.module';

import { FrontPageComponent }           from './front-page.component';
import { DeviceListModuleComponent }    from './device-list-module.component';
import { ProcessListModuleComponent }   from './process-list-module.component';
import { ScreenshotModuleComponent }    from './screenshot-module.component';

@NgModule({
    declarations: [
        AppComponent,
        FrontPageComponent,
        DeviceListModuleComponent,
        ProcessListModuleComponent,
        ScreenshotModuleComponent
    ],
    imports: [
        BrowserModule,
        FormsModule,
        HttpModule,
        AppRoutingModule
    ],
    providers: [],
    bootstrap: [AppComponent]
})
export class AppModule { }
