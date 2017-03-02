import { Component }    from '@angular/core';
import { Router }       from '@angular/router';

@Component({
    moduleId: module.id,
    selector: 'site-header',
    templateUrl: './site-header.component.html'
})
export class SiteHeaderComponent {
    constructor(private router: Router) {

    }
}
