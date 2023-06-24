import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';

import { DeviceListComponent } from './components/device-list/device-list.component';
import { HomeComponent } from './components/home/home.component';
import { ProcessListComponent } from './components/process-list/process-list.component';
import { ScreenshotComponent } from './components/screenshot/screenshot.component';
import { SystemInformationComponent } from './components/system-information/system-information.component';

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
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }
