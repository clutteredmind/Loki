import { ModuleWithProviders }          from '@angular/core';
import { Routes, RouterModule }         from '@angular/router';

import { DeviceListComponent }          from './components/pages/device-list/device-list.component';
import { HomeComponent }                from './components/pages/home/home.component';
import { ProcessListComponent }         from './components/pages/process-list/process-list.component';
import { ScreenshotComponent }          from './components/pages/screenshot/screenshot.component';
import { SystemInformationComponent }   from './components/pages/system-information/system-information.component';

const appRoutes: Routes = [
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

export const routing: ModuleWithProviders = RouterModule.forRoot(appRoutes);
