SABRE_EventTrigger event1 =
{
    SABRE_EVENT_PRESENCE_TOGGLE,
    LVL1_CASH_MACHINE_EVENT,
    { 1, 8 },
    { 2, 9 },
    0.0f, 45.0f,
    0
};

SABRE_EventTrigger event2 =
{
    SABRE_EVENT_PRESENCE_TOGGLE,
    LVL1_WINDOW_EVENT,
    { 0, 5 },
    { 2.6, 6 },
    0.0f, -30.0f
};

SABRE_EventTrigger *SABRE_updatedTrigger = NULL;

void UpdateEvents();
void SABRE_UpdateEvent(SABRE_EventTrigger *event);
void SABRE_EnterEventTrigger(SABRE_EventTrigger *event);
void SABRE_StayInEventTrigger(SABRE_EventTrigger *event);
void SABRE_LeaveEventTrigger(SABRE_EventTrigger *event);

#if DEBUG
void SABRE_ResetEvent(SABRE_EventTrigger *event);
void SABRE_CycleEventType(SABRE_EventTrigger *event);
#endif

void SABRE_UpdateEvent(SABRE_EventTrigger *event)
{
    SABRE_Vector2 camPos = SABRE_camera.pos;
    SABRE_Vector2 prevPos = SABRE_camera.prevPos;

    float deltaTo180 = 180.0f - event->facingDir;
    float normalizedDir = SABRE_NormalizeAngleTo360(direction(0, 0, SABRE_camera.dir.x, SABRE_camera.dir.y) + deltaTo180);
    float normalizedPrevDir = SABRE_NormalizeAngleTo360(direction(0, 0, SABRE_camera.prevDir.x, SABRE_camera.prevDir.y) + deltaTo180);

    short isFacingRightDirection = (event->facingFov < 0 || (abs(normalizedDir - 180.0f) <= event->facingFov / 2.0f));
    short wasFacingRightDirection = (event->facingFov < 0 || (abs(normalizedPrevDir - 180.0f) <= event->facingFov / 2.0f));

    short isInsideTrigger = (camPos.x >= event->p1.x &&
                                camPos.x <= event->p2.x &&
                                camPos.y >= event->p1.y &&
                                camPos.y <= event->p2.y);
    short wasInsideTrigger = (prevPos.x >= event->p1.x &&
                                prevPos.x <= event->p2.x &&
                                prevPos.y >= event->p1.y &&
                                prevPos.y <= event->p2.y);

    if (isFacingRightDirection && isInsideTrigger && (!wasFacingRightDirection && isFacingRightDirection || !wasInsideTrigger && isInsideTrigger))
    {
        SABRE_EnterEventTrigger(event);
    }
    else if ((!isFacingRightDirection || !isInsideTrigger) && (wasFacingRightDirection && wasInsideTrigger))
    {
        SABRE_LeaveEventTrigger(event);
    }
    else if (wasFacingRightDirection && isFacingRightDirection && wasInsideTrigger && isInsideTrigger)
    {
        SABRE_StayInEventTrigger(event);
    }
}

void SABRE_UpdateEvents()
{
    // TODO: Function that updates the events associated with the tile the player is standing in.
    SABRE_UpdateEvent(&event1);
    SABRE_UpdateEvent(&event2);
}

#if DEBUG
void SABRE_ResetEvent(SABRE_EventTrigger *event)
{
    event->state = 0;
}

void SABRE_CycleEventType(SABRE_EventTrigger *event)
{
    event->type++;
    if (event->type > 4) event->type = 0;
}
#endif

void SABRE_EnterEventTrigger(SABRE_EventTrigger *event)
{
    switch (event->type)
    {
        case SABRE_EVENT_PERMANENT_CHANGE:
            if (SABRE_EVENT_NOT_ACTIVE(event))
            {
                SABRE_EVENT_ACTIVATE(event);
            }
            break;
        case SABRE_EVENT_PRESENCE_TOGGLE:
            SABRE_EVENT_ACTIVATE(event);
            break;
        case SABRE_EVENT_PRESENCE_TOGGLE_ONCE:
            if (SABRE_EVENT_NOT_LOCKED(event))
            {
                SABRE_EVENT_ACTIVATE(event);
                SABRE_EVENT_LOCK(event);
            }
            break;
        case SABRE_EVENT_SIGNAL:
            if (SABRE_EVENT_NOT_LOCKED(event))
            {
                SABRE_EVENT_ACTIVATE(event);
                SABRE_EVENT_LOCK(event);
            }
            else
            {
                SABRE_EVENT_DEACTIVATE(event);
            }
            break;
        case SABRE_EVENT_SIGNAL_ONCE:
            if (SABRE_EVENT_NOT_LOCKED(event))
            {
                SABRE_EVENT_ACTIVATE(event);
                SABRE_EVENT_LOCK(event);
            }
            else
            {
                SABRE_EVENT_DEACTIVATE(event);
            }
            break;
    }
}

void SABRE_StayInEventTrigger(SABRE_EventTrigger *event)
{
    switch (event->type)
    {
        case SABRE_EVENT_SIGNAL:
        case SABRE_EVENT_SIGNAL_ONCE:
            SABRE_EnterEventTrigger(event);
            break;
        default:
            break;
    }
}

void SABRE_LeaveEventTrigger(SABRE_EventTrigger *event)
{
    switch (event->type)
    {
        case SABRE_EVENT_PRESENCE_TOGGLE:
            SABRE_EVENT_DEACTIVATE(event);
            break;
        case SABRE_EVENT_PRESENCE_TOGGLE_ONCE:
            SABRE_EVENT_DEACTIVATE(event);
            break;
        case SABRE_EVENT_SIGNAL:
            if (SABRE_EVENT_LOCKED(event))
            {
                SABRE_EVENT_UNLOCK(event);
            }
            break;
        case SABRE_EVENT_SIGNAL_ONCE:
            if (SABRE_EVENT_LOCKED(event))
            {
                SABRE_EVENT_DEACTIVATE(event);
            }
            break;
    }
}
