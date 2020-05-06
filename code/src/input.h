#pragma once


enum e_sproutInputKey
{
    KEY_UP,
    KEY_DOWN,
    KEY_LEFT,
    KEY_RIGHT,
    KEY_ENTER,
    KEY_SPACE,
    KEY_ESCAPE,
    INPUT_COUNT
};

struct sproutInputState
{
    void clear()
    {
        for(int i = 0; i < INPUT_COUNT; i++)
        {
            _inputa[i] = _inputb[i] = false;
        }
        currentInput = _inputa;
        previousInput = _inputb;
    }
    
    bool isKeyPressed(e_sproutInputKey key)     { return currentInput[key]; }
    bool isKeyTriggered(e_sproutInputKey key)   { return currentInput[key]  && !previousInput[key]; }
    bool isKeyReleased(e_sproutInputKey key)    { return !currentInput[key] && previousInput[key]; }

    bool * currentInput = nullptr;
    bool * previousInput = nullptr;
    bool _inputa[INPUT_COUNT];
    bool _inputb[INPUT_COUNT];
};