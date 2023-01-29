import { setFilter } from '../reducers/filterReducer'
import { connect } from 'react-redux'

const Filter = (props) => {
    const onChange = (event) => {
        event.preventDefault()
        const filter = event.target.value
        props.setFilter(filter)
    }
    const style = {
        marginBottom: 10
      }
    return(
        <div style={style}>
            Filter
            <input name='filter' onChange={onChange} />
        </div>
    )
}

const mapDispatchToProps = {
    setFilter
}

const ConnectedFilter = connect(null, mapDispatchToProps)(Filter)

export default ConnectedFilter