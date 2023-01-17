using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemySpawnManager : MonoBehaviour
{
    // List of different spawnable enemies
    public GameObject[] enemyPrefabs;

    // Range of enemy spawn positions
    public float spawnPosX = 20;
    public float spawnPosYMin = -6;
    public float spawnPosYMax = 9.5f;

    // The delay at which the enemies are spawned
    private float startDelay = 2;
    private float spawnDelay = 2.5f;

    private GameManager game;

    // Start is called before the first frame update
    void Start()
    {
        game = GameObject.FindGameObjectWithTag("GameManager").GetComponent<GameManager>();
        InvokeRepeating("SpawnEnemy", startDelay, spawnDelay);
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    void SpawnEnemy()
    {
        // Randomly choose an enemy to spawn
        int enemyIndex = Random.Range(0, enemyPrefabs.Length);
        GameObject enemy = enemyPrefabs[enemyIndex];

        // enemyPrefabs[enemyIndex].GetComponent<EnemyBat>().Spawn();
        // Randomly choose a location in the defined range
        float spawnPosY = Random.Range(game.lowerBound, game.upperBound);
        Vector3 spawnPos = new(game.rightBound + 3, spawnPosY, 0);

        Instantiate(enemy, spawnPos, enemy.transform.rotation);
    }
}
