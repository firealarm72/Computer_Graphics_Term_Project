using UnityEngine;
using System.Collections;

public class PlayerControl : MonoBehaviour
{
    public GameControlScript control;
    CharacterController controller;
    public float speed = 6.0f;
    public float boostSpeed = 8.0f;
    public float gravity = 20.0f;
    private Vector3 moveDirection = Vector3.zero;

   

    void Start()
    {
        controller = GetComponent<CharacterController>();
    }

    void Update()
    {
        if(Input.touchCount > 0)
        {
            //var touch = Input.GetMouseButtonDown(0);
            var touch = Input.GetTouch(0);
            if(touch.position.x < Screen.width / 2) //left
            {
                moveDirection = new Vector3(-1,0,0);  //get keyboard input to move in the horizontal direction
                moveDirection = transform.TransformDirection(moveDirection);  //apply this direction to the character
               // moveDirection *= speed;
            }
            else if (touch.position.x > Screen.width / 2)
            {
                moveDirection = new Vector3(1, 0, 0);  //get keyboard input to move in the horizontal direction
                moveDirection = transform.TransformDirection(moveDirection);  //apply this direction to the character

            }
        }
        controller.Move(moveDirection * Time.deltaTime);


    }

    //check if the character collects the powerups or the snags
    void OnTriggerEnter(Collider other)
    {
        if (other.gameObject.name == "Powerup(Clone)")
        {
            control.PowerupCollected();
        }
        else if (other.gameObject.name == "Obstacle4")
        {
            control.ObstacleCollected();
        }
     
        Destroy(other.gameObject);

    }
}