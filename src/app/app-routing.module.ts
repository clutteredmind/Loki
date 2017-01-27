import { NgModule }             from '@angular/core';
import { RouterModule, Routes } from '@angular/router';

import { FrontPageComponent }           from './front-page.component';
import { DeviceListModuleComponent }    from './device-list-module.component';
import { ProcessListModuleComponent }   from './process-list-module.component';
import { ScreenshotModuleComponent }    from './screenshot-module.component';

const routes: Routes = [
    { path: '', component: FrontPageComponent },
    { path: 'device-list-module', component: DeviceListModuleComponent },
    { path: 'process-list-module', component: ProcessListModuleComponent },
    { path: 'screenshot-module', component: ScreenshotModuleComponent }
];

@NgModule({
    imports: [ RouterModule.forRoot(routes) ],
    exports: [ RouterModule ]
})

export class AppRoutingModule {}
