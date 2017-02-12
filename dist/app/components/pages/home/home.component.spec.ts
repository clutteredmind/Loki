import { async, ComponentFixture, TestBed } from '@angular/core/testing';
import { By }                               from '@angular/platform-browser';
import { DebugElement }                     from '@angular/core';

import { LokiComponent }                    from '../../../interfaces/loki-component.interface';
import { SocketMessage }                    from '../../../interfaces/socket-message.interface';
import { SocketService }                    from '../../../services/socket.service';
import { ErrorDisplayComponent }            from '../../error-display/error-display.component';
import { HomeComponent }                    from './home.component';

describe('HomeComponent', () => {

    let component:      HomeComponent;
    let fixture:        ComponentFixture<HomeComponent>;
    let debugElement:   DebugElement;
    let element:        HTMLElement;

    // a fake socket service
    let socketServiceStub: {
        register(addon: LokiComponent): void;
        unregister(addonToRemove: LokiComponent): void;
        sendMessage(message: SocketMessage): void;
    };

    // async beforeEach
    beforeEach(async(() => {
        socketServiceStub = {
            register(addon: LokiComponent): void {

            },

            unregister(addonToRemove: LokiComponent): void {

            },

            sendMessage(message: SocketMessage): void {
                switch(message.action) {
                    case 'getVersionNumber':
                        component.processMessage({
                            component: 'home',
                            specifier: undefined,
                            action: 'getVersionNumber',
                            data: '1.2.3',
                            parameters: undefined
                        });
                        break;
                }
            }
        }
        TestBed.configureTestingModule({
            declarations: [
                HomeComponent,
                ErrorDisplayComponent
            ],
            providers: [ {provide: SocketService, useValue: socketServiceStub} ]
        }).compileComponents();
    }));

    // synchronous beforeEach
    beforeEach(() => {
        fixture = TestBed.createComponent(HomeComponent);

        component = fixture.componentInstance;
    });

    it('should have no errors at startup', () => {
        expect(component.errors.length).toBe(0);
    });

    it('should not display the version at startup', () => {
        debugElement = fixture.debugElement.query(By.css('p.version-display'));
        expect(debugElement).toBeNull();
    });

    it('should request and display the version number at startup', () => {
        debugElement = fixture.debugElement.query(By.css('p.version-display'));
        expect(debugElement).toBeNull();
        fixture.detectChanges();
        debugElement = fixture.debugElement.query(By.css('p.version-display'));
        element = debugElement.nativeElement;
        expect(element).not.toBeNull();
    });
});
