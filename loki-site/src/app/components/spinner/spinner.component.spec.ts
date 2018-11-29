import { By } from '@angular/platform-browser';
import { DebugElement } from '@angular/core';
import { ComponentFixture, TestBed } from '@angular/core/testing';

import { SpinnerComponent } from './spinner.component';

describe('SpinnerComponent', () => {
    let component: SpinnerComponent;
    let fixture: ComponentFixture<SpinnerComponent>;
    let debugElement: DebugElement;
    let element: HTMLElement;

    beforeEach(() => {
        TestBed.configureTestingModule({
            declarations: [SpinnerComponent], // declare the test component
        });

        fixture = TestBed.createComponent(SpinnerComponent);

        component = fixture.componentInstance;
    });

    it('should not be visible at creation', () => {
        expect(component.visible).toEqual(false);
        // try to find the spinner in the page's HTML
        debugElement = fixture.debugElement.query(By.css('i.fa'));
        expect(debugElement).toBeNull();
    });

    it('should become visible when visible=true', () => {
        component.visible = true;
        expect(component.visible).toEqual(true);
        fixture.detectChanges();
        // look for the spinner in the page's HTML
        debugElement = fixture.debugElement.query(By.css('i.fa'));
        element = debugElement.nativeElement;
        expect(element).not.toBeNull();
    });
});
