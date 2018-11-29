import { Component, OnInit, Input } from '@angular/core';

@Component({
    selector: 'app-spinner',
    template: '<i class="fa fa-cog fa-spin fa-3x fa-fw" *ngIf="visible"></i>',
    styleUrls: ['./spinner.component.css']
})
export class SpinnerComponent implements OnInit {
    @Input() visible = false;

    constructor() { }

    ngOnInit() {
    }

}
