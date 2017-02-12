import { async, ComponentFixture, TestBed } from '@angular/core/testing';
import { By }                               from '@angular/platform-browser';
import { DebugElement }                     from '@angular/core';
import { RouterTestingModule }              from '@angular/router/testing';

import { NavbarComponent }                  from './navbar.component';
import { ErrorDisplayComponent }            from '../error-display/error-display.component';

describe('NavbarComponent', () => {

    let component:      NavbarComponent;
    let fixture:        ComponentFixture<NavbarComponent>;
    let debugElement:   DebugElement;
    let element:        HTMLElement;

    // async beforeEach
    beforeEach(async(() => {
        TestBed.configureTestingModule({
            imports: [
                RouterTestingModule
            ],
            declarations: [
                NavbarComponent,
                ErrorDisplayComponent
            ]
        }).compileComponents();
    }));

    // synchronous beforeEach
    beforeEach(() => {
        fixture = TestBed.createComponent(NavbarComponent);
    });

    it('should contain the application`s title', async(() => {
        debugElement = fixture.debugElement.query(By.css('a.navbar-brand'));
        element = debugElement.nativeElement;
        expect(element.innerText).toBe('Loki');
    }));
});
