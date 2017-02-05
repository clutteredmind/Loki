import { Component, Input } from '@angular/core';

@Component({
    moduleId: module.id,
    selector: 'spinner',
    template: '<i *ngIf="visible" class="glyphicon glyphicon-cog fa-spin fa-3x fa-fw"></i>'
})
export class SpinnerComponent {
    @Input() visible = true;
}
