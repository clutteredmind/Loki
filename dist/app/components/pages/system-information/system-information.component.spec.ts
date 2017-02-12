import { async, ComponentFixture, TestBed } from '@angular/core/testing';
import { By }                               from '@angular/platform-browser';
import { DebugElement }                     from '@angular/core';

import { LokiComponent }                    from '../../../interfaces/loki-component.interface';
import { SocketMessage }                    from '../../../interfaces/socket-message.interface';
import { SocketService }                    from '../../../services/socket.service';
import { ErrorDisplayComponent }            from '../../error-display/error-display.component';
import { SystemInformationComponent }       from './system-information.component';

describe('SystemInformationComponent', () => {

    let component:      SystemInformationComponent;
    let fixture:        ComponentFixture<SystemInformationComponent>;
    let debugElement:   DebugElement;
    let element:        HTMLElement;

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

            }
        }
        TestBed.configureTestingModule({
            declarations: [
                SystemInformationComponent,
                ErrorDisplayComponent
            ],
            providers: [ {provide: SocketService, useValue: socketServiceStub} ]
        }).compileComponents();
    }));

    // synchronous beforeEach
    beforeEach(() => {
        fixture = TestBed.createComponent(SystemInformationComponent);

        component = fixture.componentInstance;
    });

    it('should have no list-group-items visible at startup', () => {
        // try to find the error display in the page's HTML
        debugElement = fixture.debugElement.query(By.css('li.list-group-item'));
        expect(debugElement).toBeNull();
    });
});
