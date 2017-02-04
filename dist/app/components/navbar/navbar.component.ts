import { Component } from '@angular/core';
import { Router }    from '@angular/router';

@Component({
    moduleId: module.id,
    selector: 'navbar',
    templateUrl: './navbar.component.html'
})
export class NavbarComponent {
    constructor(private router: Router) {

    }
}
