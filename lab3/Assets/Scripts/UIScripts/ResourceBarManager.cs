using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ResourceBarManager : MonoBehaviour
{
    public Slider slider;

    public void SetMaxValue(float value)
    {
        slider.maxValue = value;
        slider.value = value;
    }

    public void SetValue(float value)
    { 
        slider.value = value; 
    }

    public void ModifyValue(float value)
    {
        slider.value += value;
    }

    public float GetValue() 
    { 
        return slider.value; 
    }
}
