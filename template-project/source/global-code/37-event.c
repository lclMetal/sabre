#define SABRE_EVENT_PERMANENT_CHANGE      0
#define SABRE_EVENT_PRESENCE_TOGGLE       1
#define SABRE_EVENT_PRESENCE_TOGGLE_ONCE  2
#define SABRE_EVENT_SIGNAL                3
#define SABRE_EVENT_SIGNAL_ONCE           4

#define SABRE_EVENT_FLAG_ACTIVE 1
#define SABRE_EVENT_FLAG_LOCKED 2

#define SABRE_TRIGGER_HANDLER "SABRE_TriggerHandler"

#define SABRE_EVENT_ACTIVATE(EVENT) do { EVENT->state |= SABRE_EVENT_FLAG_ACTIVE;   \
                                         SABRE_updatedTrigger = EVENT;              \
                                         SendActivationEvent(SABRE_TRIGGER_HANDLER); } while (0)
#define SABRE_EVENT_DEACTIVATE(EVENT) do { EVENT->state &= (~SABRE_EVENT_FLAG_ACTIVE);  \
                                           SABRE_updatedTrigger = EVENT;                \
                                           SendActivationEvent(SABRE_TRIGGER_HANDLER); } while (0)

#define SABRE_EVENT_LOCK(EVENT) do { EVENT->state |= SABRE_EVENT_FLAG_LOCKED; } while (0)
#define SABRE_EVENT_UNLOCK(EVENT) do { EVENT->state &= (~SABRE_EVENT_FLAG_LOCKED); } while (0)

#define SABRE_EVENT_ACTIVE(EVENT) (EVENT->state & SABRE_EVENT_FLAG_ACTIVE)
#define SABRE_EVENT_NOT_ACTIVE(EVENT) (EVENT->state & SABRE_EVENT_FLAG_ACTIVE) == 0

#define SABRE_EVENT_LOCKED(EVENT) (EVENT->state & SABRE_EVENT_FLAG_LOCKED)
#define SABRE_EVENT_NOT_LOCKED(EVENT) (EVENT->state & SABRE_EVENT_FLAG_LOCKED) == 0

enum SABRE_EventNameEnum
{
    LVL1_CASH_MACHINE_EVENT = 0,
    LVL1_WINDOW_EVENT
};

struct SABRE_EventTriggerStruct
{
    unsigned char type;
    enum SABRE_EventNameEnum event;
    struct SABRE_Vector2Struct p1;
    struct SABRE_Vector2Struct p2;
    float facingDir;
    float facingFov;
    unsigned char state;
};

struct SABRE_EventTriggerStruct event1 =
{
    SABRE_EVENT_PRESENCE_TOGGLE,
    LVL1_CASH_MACHINE_EVENT,
    { 1, 8 },
    { 2, 9 },
    0.0f, 45.0f,
    0
};

struct SABRE_EventTriggerStruct event2 =
{
    SABRE_EVENT_PRESENCE_TOGGLE,
    LVL1_WINDOW_EVENT,
    { 0, 5 },
    { 2.6, 6 },
    0.0f, -30.0f
};

struct SABRE_EventTriggerStruct *SABRE_updatedTrigger = NULL;

void UpdateEvents();
void SABRE_UpdateEvent(struct SABRE_EventTriggerStruct *event);
void SABRE_EnterEventTrigger(struct SABRE_EventTriggerStruct *event);
void SABRE_StayInEventTrigger(struct SABRE_EventTriggerStruct *event);
void SABRE_LeaveEventTrigger(struct SABRE_EventTriggerStruct *event);

#if DEBUG
void SABRE_ResetEvent(struct SABRE_EventTriggerStruct *event);
void SABRE_CycleEventType(struct SABRE_EventTriggerStruct *event);
#endif

void SABRE_UpdateEvent(struct SABRE_EventTriggerStruct *event)
{
    struct SABRE_Vector2Struct camPos = SABRE_camera.pos;
    struct SABRE_Vector2Struct prevPos = SABRE_camera.prevPos;

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
void SABRE_ResetEvent(struct SABRE_EventTriggerStruct *event)
{
    event->state = 0;
}

void SABRE_CycleEventType(struct SABRE_EventTriggerStruct *event)
{
    event->type++;
    if (event->type > 4) event->type = 0;
}
#endif

void SABRE_EnterEventTrigger(struct SABRE_EventTriggerStruct *event)
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

void SABRE_StayInEventTrigger(struct SABRE_EventTriggerStruct *event)
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

void SABRE_LeaveEventTrigger(struct SABRE_EventTriggerStruct *event)
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
