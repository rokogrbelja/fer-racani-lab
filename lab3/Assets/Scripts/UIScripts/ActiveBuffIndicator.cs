using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ActiveBuffIndicator : MonoBehaviour
{
    public enum Buff
    {
        SHIELD_BUFF = 0,
        DMG_BUFF = 1
    }

    public GameObject[] indicators;

    public bool[] activeBuffs = { false, false };

    public void SetBuffActive(bool activeStatus, int id)
    {
        activeBuffs[id] = activeStatus;
        Image sprite = indicators[id].GetComponent<Image>();

        if (activeStatus == true)
            sprite.color = new(sprite.color.r, sprite.color.g, sprite.color.b, 1.0f);
        else
            sprite.color = new(sprite.color.r, sprite.color.g, sprite.color.b, 0.0f);
    }
}
