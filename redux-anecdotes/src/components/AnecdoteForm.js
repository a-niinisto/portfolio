import { setNotification } from '../reducers/notificationReducer'
import { createAnecdote } from '../reducers/anecdoteReducer'
import { connect } from 'react-redux'

const Anecdoteform = (props) => {

    const createNewAnecdote = async (event) => {
        event.preventDefault()
        const content = event.target.anecdote.value
        event.target.anecdote.value = ''
        props.createAnecdote(content)
        props.setNotification(`New anecdote: "${content}" added.`, 5)
      }
    return (
    <div>
        <h2>create new</h2>
    <form onSubmit={createNewAnecdote}>
        <div><input name='anecdote' /></div>
        <button>create</button>
      </form>
      </div>)
}
const mapDispatchToProps = {
  createAnecdote,
  setNotification
}

const ConnectedAnecdoteform = connect(null, mapDispatchToProps)(Anecdoteform)

export default ConnectedAnecdoteform