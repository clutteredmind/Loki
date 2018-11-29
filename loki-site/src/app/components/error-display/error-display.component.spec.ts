import { DebugElement } from '@angular/core';
import { By } from '@angular/platform-browser';
import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { ErrorDisplayComponent } from './error-display.component';

describe('ErrorDisplayComponent', () => {
    let component: ErrorDisplayComponent;
    let fixture: ComponentFixture<ErrorDisplayComponent>;
    let debugElement: DebugElement;
    let element: HTMLElement;

    // async beforeEach
    beforeEach(async(() => {
        TestBed.configureTestingModule({ declarations: [ErrorDisplayComponent] })
            .compileComponents();
    }));

    // synchronous beforeEach
    beforeEach(() => {
        fixture = TestBed.createComponent(ErrorDisplayComponent);

        component = fixture.componentInstance;
    });

    it('should have an empty errors array at startup', () => {
        expect(component.errors.length).toEqual(0);
    });

    it('should accept new errors', () => {
        expect(component.errors.length).toEqual(0);
        component.errors.push('An error.');
        expect(component.errors.length).toEqual(1);
    });

    it('should not be visible at startup', () => {
        // try to find the error display in the page's HTML
        debugElement = fixture.debugElement.query(By.css('div.alert'));
        expect(debugElement).toBeNull();
    });

    it('should be visible when there are errors', async(() => {
        component.errors.push('An error.');
        fixture.detectChanges();
        debugElement = fixture.debugElement.query(By.css('div.alert'));
        element = debugElement.nativeElement;
        expect(element).not.toBeNull();
    }));

    it('should allow errors to be dismissed', async(() => {
        expect(component.errors.length).toEqual(0);
        component.errors.push('An error.');
        fixture.detectChanges();
        expect(component.errors.length).toEqual(1);
        debugElement = fixture.debugElement.query(By.css('span'));
        element = debugElement.nativeElement;
        expect(element).not.toBeNull();
        element.click();
        fixture.detectChanges();
        expect(component.errors.length).toEqual(0);
    }));
});
