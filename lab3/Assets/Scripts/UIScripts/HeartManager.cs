using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class HeartManager : MonoBehaviour
{
    public GameObject[] hearts;
    public int maxHp;
    public int hp;

    public void SetMaxHp(int amount)
    {
        maxHp = amount;
        hp = amount;
    }

    public void RemoveHearts(int amount)
    {
        if (hp > 0)
        {
            hp -= amount;
            Image sprite = hearts[hp].GetComponent<Image>();
            sprite.color = new(sprite.color.r, sprite.color.g, sprite.color.b, 0.3f);
        }
    }

    public void AddHearts(int amount)
    {
        if (hp < maxHp)
        {
            hp += amount;
            Image sprite = hearts[hp - 1].GetComponent<Image>();
            sprite.color = new(sprite.color.r, sprite.color.g, sprite.color.b, 1.0f);
        }
    }
}
