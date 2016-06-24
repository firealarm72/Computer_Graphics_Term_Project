using UnityEngine;
using System.Collections;

public class PlayerControl : MonoBehaviour
{
    public GameControlScript control;
    CharacterController controller;
    public float speed = 6.0f;
    public float boostSpeed = 8.0f;
    public float gravity = 10;
    private Vector3 moveDirection = Vector3.zero;
    private bool crash = false;
    bool isGrounded = true;
    public AudioSource powerupCollectSound;
    public AudioSource obstaclecollectSound;

    void Start()
    {
        controller = GetComponent<CharacterController>();

       // controller.Move(moveDirection);
    }

    void Update()
    {

        if (isGrounded)
        {



            //float h = Input.GetAxis("Horizontal");
            if (Input.GetMouseButtonDown(0))
            {
                var touch = Input.mousePosition;
                if (touch.x < Screen.width / 2)
                {
                    moveDirection = new Vector3(-1, 0, 0);  //get keyboard input to move in the horizontal direction
                    moveDirection = transform.TransformDirection(moveDirection);  //apply this direction to the character
                                                                                  // moveDirection *= speed;
                }
                else if (touch.x > Screen.width / 2)
                {
                    moveDirection = new Vector3(1, 0, 0);  //get keyboard input to move in the horizontal direction
                    moveDirection = transform.TransformDirection(moveDirection);  //apply this direction to the character

                }
            }
            if (Input.GetMouseButtonDown(1))
            {
                moveDirection = new Vector3(0, 3, 0);
                isGrounded = false;
            }
            if (controller.isGrounded)
            {
                isGrounded = true;
            }

            if (control.isGameOver)
            {
                //gameObject.GetComponent<AudioSource>().enabled = false;
            }


            moveDirection.y -= gravity * Time.deltaTime*0.3f;
            controller.Move(moveDirection);
            moveDirection = Vector3.zero;
        }
    }

    //check if the character collects the powerups or the snags
    void OnTriggerEnter(Collider other)
    {
        if (other.gameObject.name == "Powerup(Clone)")
        {
            control.PowerupCollected();
            powerupCollectSound.Play();
        }
        else if (other.gameObject.name == "Obstacle4(Clone)" && isGrounded == true)
        {
           
                control.ObstacleCollected();
            obstaclecollectSound.Play();
            

        }

        Destroy(other.gameObject);

    }
}