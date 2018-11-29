import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';

import { HomeComponent } from './components/pages/home/home.component'
import { DeviceListComponent } from './components/pages/device-list/device-list.component';
import { ProcessListComponent } from './components/pages/process-list/process-list.component';
import { ScreenshotComponent } from './components/pages/screenshot/screenshot.component';
import { SystemInformationComponent } from './components/pages/system-information/system-information.component';


const routes: Routes = [
    {
        path: '',
        component: HomeComponent
    },
    {
        path: 'device-list',
        component: DeviceListComponent
    },
    {
        path: 'process-list',
        component: ProcessListComponent
    },
    {
        path: 'screenshot',
        component: ScreenshotComponent
    },
    {
        path: 'system-information',
        component: SystemInformationComponent
    },
    {
        path: '**',
        redirectTo: '/'
    }
];

@NgModule({
    imports: [
        RouterModule.forRoot(routes)
    ],
    exports: [
        RouterModule
    ]
})
export class AppRoutingModule { }
