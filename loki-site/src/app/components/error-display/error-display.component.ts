import { Component, OnInit, Input } from '@angular/core';

@Component({
    selector: 'app-error-display',
    templateUrl: './error-display.component.html',
    styleUrls: ['./error-display.component.css']
})
export class ErrorDisplayComponent implements OnInit {

    @Input() errors: Array<string>;

    constructor() {
        this.errors = new Array<string>();
    }

    dismissError(errorId: number): void {
        this.errors.splice(errorId, 1);
    }

    ngOnInit() { }
}
