import { Component, Input } from '@angular/core';

@Component({
    moduleId: module.id,
    selector: 'error-display',
    templateUrl: './error-display.component.html'
})
export class ErrorDisplayComponent {
    @Input() errors: Array<string>;

    dismissError(errorId: number): void {
        this.errors.splice(errorId, 1);
    }
}
