import { Router } from '@angular/router';
import { Component, OnInit } from '@angular/core';

@Component({
    selector: 'app-site-sidebar',
    templateUrl: './site-sidebar.component.html',
    styleUrls: ['./site-sidebar.component.css']
})
export class SiteSidebarComponent implements OnInit {

    constructor(private router: Router) {
    }

    ngOnInit() {
    }
}
