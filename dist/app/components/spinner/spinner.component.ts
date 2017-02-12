import { Component, Input } from '@angular/core';

@Component({
    moduleId: module.id,
    selector: 'spinner',
    template: '<i class="fa fa-cog fa-spin fa-3x fa-fw" *ngIf="visible"></i>'
})
export class SpinnerComponent {
    @Input() visible = false;
}
