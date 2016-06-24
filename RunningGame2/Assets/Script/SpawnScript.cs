using UnityEngine;
using System.Collections;

public class SpawnScript : MonoBehaviour
{
    public GameObject obstacle;
    public GameObject powerup;

    float timeElapsed = 0;
    float spawnCycle = 0.9f;
    bool spawnPowerup = true;

    void Update()
    {
        timeElapsed += Time.deltaTime;
        if (timeElapsed > spawnCycle)
        {
            GameObject temp;
            if (spawnPowerup)
            {
                temp = (GameObject)Instantiate(powerup);
                Vector3 pos = temp.transform.position;
                temp.transform.position = new Vector3(Random.Range(-3, 4), pos.y+1, pos.z+30);
            }
            else
            {
                temp = (GameObject)Instantiate(obstacle);
                Vector3 pos = temp.transform.position;
                temp.transform.position = new Vector3(Random.Range(-3, 4), pos.y+1.3f, pos.z+30);
                //temp.transform.rotation = new Quaternion(0, 500, 0, 0);

            }

            timeElapsed -= spawnCycle;
            spawnPowerup = !spawnPowerup;
        }
    }
}