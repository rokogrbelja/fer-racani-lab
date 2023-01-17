using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[CreateAssetMenu(fileName = "Data", menuName = "ScriptableObjects/Enemy", order = 1)]
public class EnemyData : ScriptableObject
{
    public float hp;
    public float speed;
    public float sightRange;
    public float attackRange;
    public int scoreValue;

    public AudioClip attackSound;
}
