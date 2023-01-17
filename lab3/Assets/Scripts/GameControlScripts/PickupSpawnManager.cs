using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PickupSpawnManager : MonoBehaviour
{
    // List of different spawnable pickups
    public GameObject[] pickupPrefabs;

    // Range of enemy spawn positions
    public float spawnPosX = 20;
    public float spawnPosYMin = -6;
    public float spawnPosYMax = 9.5f;

    // The delay at which the enemies are spawned
    private float startDelay = 8;
    private float spawnDelay = 12;

    private GameManager game;

    // Start is called before the first frame update
    void Start()
    {
        game = GameObject.FindGameObjectWithTag("GameManager").GetComponent<GameManager>();
        InvokeRepeating("SpawnPickup", startDelay, spawnDelay);
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    private void SpawnPickup()
    {
        // Randomly choose a pickup to spawn
        int pickupIndex = Random.Range(0, pickupPrefabs.Length);
        GameObject pickup = pickupPrefabs[pickupIndex];

        // Randomly choose a location in the defined range
        float spawnPosY = Random.Range(game.lowerBound, game.upperBound);
        Vector3 spawnPos = new(game.rightBound + 3, spawnPosY, 0);

        Instantiate(pickup, spawnPos, pickup.transform.rotation);
    }
}
