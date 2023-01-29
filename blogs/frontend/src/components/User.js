const User = (props) => (
    <div>
      {props.user.name} Logged in
      <button onClick={props.logout}>logout</button>
    </div>
  )
  export default User