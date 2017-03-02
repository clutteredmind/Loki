import { Component }    from '@angular/core';
import { Router }       from '@angular/router';

@Component({
    moduleId: module.id,
    selector: 'site-sidebar',
    templateUrl: './site-sidebar.component.html'
})
export class SiteSidebarComponent {
    constructor(private router: Router) {

    }
}
